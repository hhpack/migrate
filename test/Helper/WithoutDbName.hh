<?hh //strict

namespace HHPack\Migrate\Test\Helper\Db;

use type HHPack\Migrate\Database\{Connection, DatabaseClient, DatabaseServer, QueryResult};
use type Facebook\HackTest\HackTest;

trait WithoutDbName {
  require extends HackTest;

  public static ?Connection $connection = null;

  private function currentConnection() : Connection {
    if (!\is_null(static::$connection)) {
      return static::$connection;
    }

    static::$connection = \HH\Asio\join(
      DatabaseClient::createWithoutDbConnection(
        new DatabaseServer(
          (string)\getenv('DB_HOSTNAME'),
          (int)\getenv('DB_PORT'),
        ),
        (string)\getenv('DB_USER'),
        (string)\getenv('DB_PASSWORD'),
      ),
    );

    return static::$connection;
  }

  public function connectionResolve<T>((function(Connection): T) $resolver): T {
    return $resolver( $this->currentConnection() );
  }

  public async function createDatabase(string $name): Awaitable<void> {
    $conn = $this->currentConnection();
    $dbname = $conn->escapeString($name);
    await $conn->rawQuery(\sprintf("DROP DATABASE IF EXISTS %s", $dbname));
    await $conn->rawQuery(\sprintf("CREATE DATABASE %s", $dbname));
  }

  public async function dropDatabase(string $name): Awaitable<void> {
    $conn = $this->currentConnection();
    $dbname = $conn->escapeString($name);
    await $conn->rawQuery(\sprintf("DROP DATABASE IF EXISTS %s", $dbname));
  }

  public function findDatabase(string $name) : ImmVector<ImmMap<string, mixed>> {
    $conn = $this->currentConnection();

    $result = \HH\Asio\join($conn->rawQuery('show databases'));
    return $result->rows()->filter(($row) ==> $row->at('Database') === $name);
  }
}
