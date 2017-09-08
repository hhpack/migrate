<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{UpCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\{Db};
use HackPack\HackUnit\Contract\Assert;

final class UpCommandTest {

  public function __construct(
    private Connection $conn,
    private Context $context,
  ) {}

  <<Setup('test')>>
  public function setUpTest(): void {
    \HH\Asio\join(
      $this->conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations"),
    );
    \HH\Asio\join($this->conn->rawQuery("DROP TABLE IF EXISTS users"));
    \HH\Asio\join($this->conn->rawQuery("DROP TABLE IF EXISTS posts"));
  }

  <<SuiteProvider('NoArguments')>>
  public static function noArguments(): this {
    $path = File\absolutePath(__DIR__.'/../../sql/migrations/');

    $conn = Db\connect();
    $context = new MigrateContext($path, []);

    return new static($conn, $context);
  }

  <<SuiteProvider('ToOption')>>
  public static function toOptions(): this {
    $path = File\absolutePath(__DIR__.'/../../sql/migrations/');

    $conn = Db\connect();
    $context =
      new MigrateContext($path, ['--to=20150824010439-create-users']);

    return new static($conn, $context);
  }

  <<Test('NoArguments')>>
  public function migrateToLatestSchema(Assert $assert): void {
    $command = new UpCommand();
    $command->run($this->context);

    $result = \HH\Asio\join($this->conn->rawQuery('show tables'));
    $rows = $result->rows();

    // users, posts, scheme_migrations
    $assert->int($rows->count())->eq(3);
  }

  <<Test('ToOption')>>
  public function migrateToUsersSchema(Assert $assert): void {
    $command = new UpCommand();
    $command->run($this->context);

    $result = \HH\Asio\join($this->conn->rawQuery('show tables'));
    $rows = $result->rows();

    // users, scheme_migrations
    $assert->int($rows->count())->eq(2);
  }
}
