<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{DropDatabaseCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\Db\{WithoutDbName};

use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class DropDatabaseCommandTest extends HackTest {
  use WithoutDbName;

  const DB_NAME = 'TEST_DB';

  public async function beforeEachTestAsync(): Awaitable<void> {
    await $this->createDatabase(static::DB_NAME);
  }

  public function provideContext(): vec<(Context)> {
    return $this->connectionResolve((Connection $conn) ==> {
      $path = File\absolute_path(__DIR__.'/../../sql/migrations/');
      $context = new MigrateContext($path, [], static::DB_NAME);

      return vec[tuple($context)];
    });
  }

  <<DataProvider('provideContext')>>
  public function testDropDatabase(Context $context): void {
    $command = new DropDatabaseCommand();
    $command->run($context);

    $currentDatabases = $this->findDatabase(static::DB_NAME);
    expect($currentDatabases->count())->toBeSame(0);
  }
}
