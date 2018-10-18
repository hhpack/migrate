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
use HHPack\Getopt\Parser\{OptionParser};
use HHPack\Migrate\{File, Output, Logger};
use HHPack\Migrate\Application\Command\{
  CreateDatabaseCommand,
  DropDatabaseCommand,
  UpCommand,
  DownCommand,
  ResetCommand,
  GenerateMigrationCommand,
};
use HHPack\Migrate\Logger\{PlainLogger, ColoredLogger};

final class MigrateApplication {

  const PROGRAM_NAME = 'migrate';
  const PROGRAM_VERSION = '2.0.3';
  const PROGRAM_LICENCE = 'The MIT Licence';

  private bool $help = false;
  private bool $version = false;
  private bool $outputColor = true;
  private string $configurationPath = 'config/database.json';
  private OptionParser $optionParser;
  private ImmMap<string, Command> $commands;

  public function __construct(private Output $output) {
    $parserOptions = shape('stopAtNonOption' => true);

    $this->optionParser = cli\optparser(
      [
        cli\take_on(
          ['-c', '--config'],
          'FILE',
          'Path of configuration file',
          ($path) ==> {
            $this->configurationPath = $path;
          },
        ),
        cli\on(
          ['--no-color'],
          'If specified, color output will not be performed',
          () ==> {
            $this->outputColor = false;
          },
        ),
        cli\on(
          ['-h', '--help'],
          'Display help message',
          () ==> {
            $this->help = true;
          },
        ),
        cli\on(
          ['-v', '--version'],
          'Display version',
          () ==> {
            $this->version = true;
          },
        ),
      ],
      $parserOptions,
    );

    $this->commands = ImmMap {
      "create" => new CreateDatabaseCommand(),
      "drop" => new DropDatabaseCommand(),
      "up" => new UpCommand(),
      "down" => new DownCommand(),
      "reset" => new ResetCommand(),
      "gen" => new GenerateMigrationCommand(),
    };
  }

  public function run(Traversable<string> $argv): void {
    $cliArgv = Vector::fromItems($argv)->skip(1);
    $remainArgv = $this->optionParser->parse($cliArgv);

    if ($this->help) {
      $this->displayHelp();
    } else if ($this->version) {
      $this->displayVersion();
    } else if (!$remainArgv->containsKey(0)) {
      $this->displayHelp();
    } else {
      $commandName = $remainArgv->at(0);
      $this->runCommand($commandName, $remainArgv->skip(1));
    }
  }

  private function runCommand(string $name, ImmVector<string> $argv): void {
    if (!$this->commands->contains($name)) {
      $this->displayHelp();
    } else {
      $loader = new ConfigurationLoader(
        File\absolute_path(\getcwd().'/'.$this->configurationPath),
      );
      $env = \getenv('HHVM_ENV') ? \getenv('HHVM_ENV') : 'development';
      $configuration = $loader->load($env);

      $context =
        new MigrateContext($argv, $configuration, $this->outputLogger());

      $command = $this->commands->at($name);
      $command->run($context);
    }
  }

  private function outputLogger(): Logger {
    if ($this->outputColor) {
      return new ColoredLogger($this->output);
    } else {
      return new PlainLogger($this->output);
    }
  }

  private function displayHelp(): void {
    $this->output->write("Migration tool for database.\n");
    $this->output->write("https://github.com/hhpack/migrate");
    $this->output->write(
      \sprintf(
        "\n\n  %s %s %s\n\n",
        static::PROGRAM_NAME,
        "[OPTIONS]",
        "[COMMAND]",
      ),
    );

    $this->output->write("Commands:\n");
    $this->displayCommands();
    $this->output->write("\n");

    $this->optionParser->displayHelp();
  }

  private function displayCommands(): void {
    $keys = $this->commands->keys()->toValuesArray();
    $maxLength = (int)\array_reduce(
      $keys,
      (int $max, string $key) ==> {
        return ($max < \strlen($key)) ? \strlen($key) : $max;
      },
      0,
    );

    foreach ($this->commands->lazy() as $name => $command) {
      $this->output->write(
        \sprintf(
          "  %s   %s\n",
          \str_pad($name, $maxLength),
          $command->description(),
        ),
      );
    }
  }

  private function displayVersion(): void {
    $this->output->write(
      \sprintf("%s %s\n", static::PROGRAM_NAME, static::PROGRAM_VERSION),
    );
  }

}
