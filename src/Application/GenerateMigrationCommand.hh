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
use HHPack\Migrate\Migration\{ MigrationGenerator, GenerateVerison };
use HHPack\Migrate\Migration\Generator\{ SqlMigrationGenerator };
use RuntimeException;

final class GenerateMigrationCommand extends AbstractCommand implements Command
{

    public function __construct()
    {
        $this->usage = "migrate gen [OPTIONS] [NAME]";
        $this->description = "Generate a new migration file";
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
                throw new RuntimeException('Please specify the migration name');
            }

            $name = $remainArgs->at(0);

            $generator = $this->createMigrationGenerator($context);
            $generator->generate(shape(
                'version' => (string) (new \DateTime())->format('Ymdhis'),
                'name' => $name
            ));
        }
    }

    private function createMigrationGenerator(Context $context) : MigrationGenerator
    {
        $generator = new SqlMigrationGenerator(
            $context->migrationPath(),
            $context->logger()
        );

        return $generator;
    }

}
