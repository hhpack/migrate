namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{GenerateMigrationCommand};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\{Db};

use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class GenerateMigrationCommandTest extends HackTest {

  <<__Memoize>>
  public function createContext(): Context {
    $path = File\absolute_path(\sys_get_temp_dir());
    $context = new MigrateContext($path, ['create-groups']);
    return $context;
  }

  public function provideNewSqlMigration(): vec<(Context)> {
    return vec[tuple($this->createContext())];
  }

  public async function beforeEachTestAsync(): Awaitable<void> {
    $this->cleanUpSqlMigrationFiles($this->createContext());
  }

  <<DataProvider('provideNewSqlMigration')>>
  public function testGenerateMigrationFile(Context $context): void {
    $command = new GenerateMigrationCommand();
    $command->run($context);

    $entries = $this->sqlMigrationFiles($context);
    expect($entries->count())->toBeSame(2);
  }

  private function sqlMigrationFiles(Context $context): ImmSet<string> {
    $migration = $context->migration();

    $entries = Set {};
    $directory = \dir($migration->path());

    while (false !== ($entry = $directory->read())) {
      if (
        \preg_match(
          '/(create-groups.up\.sql|create-groups.down\.sql)$/',
          $entry,
        )
      ) {
        $entries->add($entry);
      }
    }

    return $entries->immutable();
  }

  private function cleanUpSqlMigrationFiles(Context $context): void {
    $migration = $context->migration();
    $directory = $migration->path();

    foreach ($this->sqlMigrationFiles($context) as $file) {
      \unlink($directory.'/'.$file);
    }
  }

}
