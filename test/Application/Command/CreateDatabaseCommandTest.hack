namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{CreateDatabaseCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\Db\{WithoutDbName};

use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class CreateDatabaseCommandTest extends HackTest {
  use WithoutDbName;

  const DB_NAME = 'TEST_DB';

  public async function beforeEachTestAsync(): Awaitable<void> {
    await $this->dropDatabase(static::DB_NAME);
  }

  public function provideContext(): vec<(MigrateContext)> {
    return $this->connectionResolve((Connection $conn) ==> {
      $path = File\absolute_path(__DIR__.'/../../sql/migrations/');
      $context = new MigrateContext($path, [], static::DB_NAME);

      return vec[tuple($context)];
    });
  }

  <<DataProvider('provideContext')>>
  public function testCreateDatabase(MigrateContext $context): void {
    $command = new CreateDatabaseCommand();
    $command->run($context);

    $createdDatabases = $this->findDatabase(static::DB_NAME);
    expect($createdDatabases->count())->toBeSame(1);
  }
}
