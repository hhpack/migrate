<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Command;

use HHPack\Getopt as cli;
use HHPack\Getopt\Parser\{ OptionParser };
use HHPack\Migrate\{ Migrator, SqlMigrationLoader };
use HHPack\Migrate\Application\{ Context, Command };

use RuntimeException;

final class DownCommand extends AbstractCommand implements Command
{

    public function __construct()
    {
        $this->usage = "migrate down [OPTIONS] [NAME]";
        $this->description = "Downgrade the schema of the database";
        $this->optionParser = cli\optparser([
            cli\on(['-h', '--help'], 'Display help message', () ==> {
                $this->help = true;
            })
        ]);
    }

    public function run(Context $context): void
    {
        $remainArgs = $this->optionParser->parse($context->cliArgs());

        if ($this->help) {
            $this->displayHelp();
        } else {
            if (!$remainArgs->containsKey(0)) {
                throw new RuntimeException('Please specify the version to downgrade');
            }

            $schemaVersion = $remainArgs->at(0);

            \HH\Asio\join($this->downgradeSchema($schemaVersion, $context));
        }
    }

    private async function downgradeSchema(string $schemaVersion, Context $context): Awaitable<void>
    {
        $mysql = $context->connectDatabase();
        $migrator = new Migrator($context->migrationLoader(), $mysql, $context->logger());

        await $migrator->downgrade($schemaVersion);
    }

}
