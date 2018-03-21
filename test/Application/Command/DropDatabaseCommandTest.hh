<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{File};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Command\{DropDatabaseCommand};
use HHPack\Migrate\Database\{DataSourceName, Connection};
use HHPack\Migrate\Test\Mock\{MigrateContext};
use HHPack\Migrate\Test\Helper\{Db};
use HackPack\HackUnit\Contract\Assert;

final class DropDatabaseCommandTest {

  const DB_NAME = 'TEST_DB';

  public function __construct(
    private Connection $conn,
    private Context $context,
  ) {}

  <<Setup('test')>>
  public function setUpTest(): void {
    $dbname = $this->conn->escapeString(static::DB_NAME);
    \HH\Asio\join(
      $this->conn->rawQuery(sprintf("DROP DATABASE IF EXISTS %s", $dbname)),
    );
    \HH\Asio\join(
      $this->conn->rawQuery(sprintf("CREATE DATABASE %s", $dbname)),
    );
  }

  <<SuiteProvider('Default')>>
  public static function noArguments(): this {
    $conn = Db\connectWithoutDbname();

    $path = File\absolute_path(__DIR__.'/../../sql/migrations/');
    $context = new MigrateContext($path, [], static::DB_NAME);

    return new static($conn, $context);
  }

  <<Test('Default')>>
  public function dropDatabase(Assert $assert): void {
    $command = new DropDatabaseCommand();
    $command->run($this->context);

    $result = \HH\Asio\join($this->conn->rawQuery('show databases'));
    $currentDatabases =
      $result->rows()
        ->filter(($row) ==> $row->at('Database') === static::DB_NAME);

    $assert->int($currentDatabases->count())->eq(0);
  }
}
