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
use HHPack\Migrate\{ Migrator, SqlMigrationLoader, DatabaseClient };

final class UpCommand extends AbstractCommand implements Command
{
    private ?string $schemaVersion = null;

    public function __construct()
    {
        $this->usage = "migrate up [OPTIONS]";
        $this->description = "Upgrade the schema of the database";
        $this->optionParser = cli\optparser([
            cli\take_on(['--to'], 'Version of the schema to be upgraded', ($version) ==> {
                $this->schemaVersion = $version;
            }),
            cli\on(['-h', '--help'], 'Display help message', () ==> {
                $this->help = true;
            })
        ]);
    }

    public function run(Context $context): void
    {
        $this->optionParser->parse($context->cliArgs());

        if ($this->help) {
            $this->displayHelp();
        } else {
            \HH\Asio\join($this->upgradeSchema($context));
        }
    }

    private async function upgradeSchema(Context $context): Awaitable<void>
    {
        $mysql = await $context->databaseClient();
        $migrator = new Migrator($context->migrationLoader(), $mysql);

        if (is_null($this->schemaVersion)) {
            await $migrator->upgrade();
        } else {
            await $migrator->upgrade($this->schemaVersion);
        }
    }

}
