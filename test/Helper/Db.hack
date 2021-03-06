namespace HHPack\Migrate\Test\Helper\Db;

use HHPack\Migrate\Application\Configuration\{Server};
use HHPack\Migrate\Database\{Connection, DatabaseClient, DatabaseServer};

function connect(): Connection {
  $host = (string)\getenv('DB_HOSTNAME');
  $name = (string)\getenv('DB_DATABASE');
  $port = (string)\getenv('DB_PORT');
  $user = (string)\getenv('DB_USER');
  $password = (string)\getenv('DB_PASSWORD');

  static $connection = null;

  if (!\is_null($connection)) {
    return $connection;
  }

  $connection = \HH\Asio\join(
    DatabaseClient::createConnection(
      'mysql:host='.$host.';port='.$port.';dbname='.$name,
      $user,
      $password,
    ),
  );

  return $connection;
}

function connectServerSetting(?string $name = null): Server {
  $defaultName = (string)\getenv('DB_DATABASE');

  return new Server(
    (string)\getenv('DB_HOSTNAME'),
    (int)\getenv('DB_PORT'),
    (\is_null($name)) ? $defaultName : $name,
    (string)\getenv('DB_USER'),
    (string)\getenv('DB_PASSWORD'),
  );
}

function connectWithoutDbname(): Connection {
  static $connection = null;

  if (!\is_null($connection)) {
    return $connection;
  }

  $connection = \HH\Asio\join(
    DatabaseClient::createWithoutDbConnection(
      new DatabaseServer(
        (string)\getenv('DB_HOSTNAME'),
        (int)\getenv('DB_PORT'),
      ),
      (string)\getenv('DB_USER'),
      (string)\getenv('DB_PASSWORD'),
    ),
  );

  return $connection;
}
