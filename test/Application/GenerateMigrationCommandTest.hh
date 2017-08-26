<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{ File };
use HHPack\Migrate\Application\{ Context, GenerateMigrationCommand };
use HHPack\Migrate\Test\Mock\{ MigrateContext };
use HHPack\Migrate\Test\Helper\{ Db };
use HackPack\HackUnit\Contract\Assert;


final class GenerateMigrationCommandTest
{

    public function __construct(
        private Context $context
    )
    {
    }

    <<SuiteProvider('SqlMigration')>>
    public static function newSqlMigration() : this
    {
        $path = File\absolutePath(sys_get_temp_dir());

        $conn = Db\connect();
        $context = new MigrateContext($path, ['create-groups'], $conn);

        return new static($context);
    }

    <<Setup('test')>>
    public function setUpTest() : void
    {
        $this->cleanUpSqlMigrationFiles();
    }

    <<Test('SqlMigration')>>
    public function generateMigrationFile(Assert $assert): void
    {
        $command = new GenerateMigrationCommand();
        $command->run($this->context);

        $entries = $this->sqlMigrationFiles();
        $assert->int($entries->count())->eq(2);
    }

    private function sqlMigrationFiles() : ImmSet<string>
    {
        $entries = Set {};
        $directory = dir($this->context->migrationPath());

        while (false !== ($entry = $directory->read())) {
            if (preg_match('/(create-groups.up\.sql|create-groups.down\.sql)$/', $entry)) {
                $entries->add($entry);
            }
        }

        return $entries->immutable();
    }

    private function cleanUpSqlMigrationFiles() : void
    {
        $directory = $this->context->migrationPath();

        foreach ($this->sqlMigrationFiles() as $file) {
            unlink($directory . '/' . $file);
        }
    }

}
