namespace HHPack\Migrate\Test\Helper\Db;

use type HHPack\Migrate\Database\{Connection, DatabaseClient};
use type Facebook\HackTest\HackTest;

trait WithDbName {
  require extends HackTest;

  public static ?Connection $connection = null;

  public function currentConnection(): Connection {
    $host = (string)\getenv('DB_HOSTNAME');
    $name = (string)\getenv('DB_DATABASE');
    $port = (string)\getenv('DB_PORT');
    $user = (string)\getenv('DB_USER');
    $password = (string)\getenv('DB_PASSWORD');

    if (!\is_null(static::$connection)) {
      return static::$connection;
    }

    static::$connection = \HH\Asio\join(
      DatabaseClient::createConnection(
        'mysql:host='.$host.';port='.$port.';dbname='.$name,
        $user,
        $password,
      ),
    );

    return static::$connection;
  }

}
