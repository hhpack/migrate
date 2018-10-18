<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{UpCommand, DownCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\{Db};

use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class DownCommandTest extends HackTest {

  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = Db\connect();

    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations"));
    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS users"));
    \HH\Asio\join($conn->rawQuery("DROP TABLE IF EXISTS posts"));
  }

  public function provideContext(): vec<(Context, Context, Connection)> {
    $path = File\absolute_path(__DIR__.'/../../sql/migrations/');

    $conn = Db\connect();
    $upContext = new MigrateContext($path, []);
    $downContext = new MigrateContext($path, ['20150824010439-create-users']);

    return vec[tuple($upContext, $downContext, $conn)];
  }

  <<DataProvider('provideContext')>>
  public function testMigrateToInitialState(
    Context $upContext,
    Context $downContext,
    Connection $conn,
  ): void {
    $command = new UpCommand();
    $command->run($upContext);

    $command = new DownCommand();
    $command->run($downContext);

    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    $rows = $result->rows();

    // scheme_migrations
    expect($rows->count())->toBeSame(1);
  }
}
