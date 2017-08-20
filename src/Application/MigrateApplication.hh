<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Getopt as cli;
use HHPack\Getopt\Parser\{ OptionParser };
use HHPack\Migrate\{ File };

final class MigrateApplication
{

    const PROGRAM_NAME = 'migrate';
    const PROGRAM_VERSION = '1.0.0';
    const PROGRAM_LICENCE = 'The MIT Licence';

    private bool $help = false;
    private bool $version = false;
    private string $configurationPath = 'config/database.json';
    private OptionParser $optionParser;
    private ImmMap<string, Command> $commands;

    public function __construct()
    {
        $this->optionParser = cli\optparser([
            cli\take_on(['-c', '--config'], 'Path of configuration file', ($path) ==> {
                $this->configurationPath = $path;
            }),
            cli\on(['-h', '--help'], 'Display help message', () ==> {
                $this->help = true;
            }),
            cli\on(['-v', '--version'], 'Display version', () ==> {
                $this->version = true;
            })
        ]);

        $this->commands = ImmMap {
            "up" => new UpCommand(),
            "down" => new DownCommand()
        };
    }

    public function run(Traversable<string> $argv): void
    {
        $cliArgv = Vector::fromItems($argv)->skip(1);
        $remainArgv = Vector::fromItems($this->optionParser->parse($cliArgv));

        if ($this->help || !$remainArgv->containsKey(0)) {
            $this->displayHelp();
        } else if ($this->version) {
            $this->displayVersion();
        } else {
            $commandName = $remainArgv->at(0);
            $this->runCommand($commandName, $argv);
        }
    }

    private function runCommand(string $name, Traversable<string> $argv): void
    {
        if (!$this->commands->contains($name)) {
            $this->displayHelp();
        } else {
            $loader = new ConfigurationLoader(File\absolutePath(getcwd() . $this->configurationPath));
            $env = getenv('HHVM_ENV') ? getenv('HHVM_ENV') : 'development';
            $configuration = $loader->load($env);

            $context = new MigrateContext($argv, $configuration);

            $command = $this->commands->at($name);
            $command->run($context);
        }
    }

    private function displayHelp(): void
    {
        fwrite(STDOUT, "Migration tool for database.\n");
        fwrite(STDOUT, "https://github.com/hhpack/migrate");
        fwrite(STDOUT, sprintf("\n\n  %s %s %s\n\n",
            static::PROGRAM_NAME,
            "[OPTIONS]", "[COMMAND]"));

        fwrite(STDOUT, "Commands:\n");
        $this->displayCommands();
        fwrite(STDOUT, "\n");

        $this->optionParser->displayHelp();
    }

    private function displayCommands(): void
    {
        $keys = $this->commands->keys()->toValuesArray();
        $maxLength = (int) array_reduce($keys, (int $max, string $key) ==> {
            return ($max < strlen($key)) ? strlen($key) : $max;
        }, 0);

        foreach ($this->commands->lazy() as $name => $command) {
            fwrite(STDOUT, sprintf("  %s   %s\n", str_pad($name, $maxLength), $command->description()));
        }
    }

    private function displayVersion(): void
    {
        fwrite(STDOUT, sprintf("%s %s\n", static::PROGRAM_NAME, static::PROGRAM_VERSION));
    }

}
