namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{UpCommand, ResetCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\Db\{WithDbName};

use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class ResetCommandTest extends HackTest {
  use WithDbName;

  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = $this->currentConnection();
    await $conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations");
    await $conn->rawQuery("DROP TABLE IF EXISTS users");
    await $conn->rawQuery("DROP TABLE IF EXISTS posts");
  }

  public function provideContext(): vec<(Connection, Context, Context)> {
    $path = File\absolute_path(__DIR__.'/../../sql/migrations/');

    $conn = $this->currentConnection();
    $upContext = new MigrateContext($path, []);
    $resetContext = new MigrateContext($path, []);

    return vec[tuple($conn, $upContext, $resetContext)];
  }

  <<DataProvider('provideContext')>>
  public function testMigrateToInitialState(
    Connection $conn,
    Context $upContext,
    Context $resetContext,
  ): void {
    $command = new UpCommand();
    $command->run($upContext);

    $command = new ResetCommand();
    $command->run($resetContext);

    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    $rows = $result->rows();

    // scheme_migrations
    expect($rows->count())->toBeSame(1);
  }
}
