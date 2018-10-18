<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{UpCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\{Db};

use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class UpCommandTest extends HackTest {

  public async function beforeEachTestAsync(): Awaitable<void> {
    $conn = Db\connect();
    await $conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations");
    await $conn->rawQuery("DROP TABLE IF EXISTS users");
    await $conn->rawQuery("DROP TABLE IF EXISTS posts");
  }

  public function provideNoArguments(): vec<(Connection, Context)> {
    $path = File\absolute_path(__DIR__.'/../../sql/migrations/');

    $conn = Db\connect();
    $context = new MigrateContext($path, []);

    return vec[tuple($conn, $context)];
  }

  public function provideOptions(): vec<(Connection, Context)> {
    $path = File\absolute_path(__DIR__.'/../../sql/migrations/');

    $conn = Db\connect();
    $context = new MigrateContext($path, ['--to=20150824010439-create-users']);

    return vec[tuple($conn, $context)];
  }

  <<DataProvider('provideNoArguments')>>
  public function testMigrateToLatestSchema(Connection $conn, Context $context): void {
    $command = new UpCommand();
    $command->run($context);

    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    $rows = $result->rows();

    // users, posts, scheme_migrations
    expect($rows->count())->toBeSame(3);
  }

  <<DataProvider('provideOptions')>>
  public function testMigrateToUsersSchema(Connection $conn, Context $context): void {
    $command = new UpCommand();
    $command->run($context);

    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    $rows = $result->rows();

    // users, scheme_migrations
    expect($rows->count())->toBeSame(2);
  }
}
