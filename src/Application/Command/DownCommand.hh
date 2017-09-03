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
use HHPack\Migrate\{ Migrator };
use HHPack\Migrate\Migration\{ MigrationLoader };
use HHPack\Migrate\Application\{ Context, Command };
use HHPack\Migrate\Application\Configuration\{ Server };
use RuntimeException;


final class DownCommand extends MigrateSchemaCommand implements Command
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

            $migrator = $this->createMigrator($context);
            \HH\Asio\join($migrator->downgrade($schemaVersion));
        }
    }
}
