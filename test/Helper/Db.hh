<?hh //strict

namespace HHPack\Migrate\Test\Helper\Db;

use HHPack\Migrate\{ Connection, DatabaseClient };

function connect(): Connection {
    $host = (string) getenv('DB_HOSTNAME');
    $name = (string) getenv('DB_DATABASE');
    $port = (string) getenv('DB_PORT');
    $user = (string) getenv('DB_USER');
    $password = (string) getenv('DB_PASSWORD');

    return \HH\Asio\join(DatabaseClient::createConnection(
        'mysql:host=' . $host . ';port=' . $port . ';dbname=' . $name,
        $user,
        $password
    ));
}
