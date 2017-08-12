<?hh //strict

namespace HHPack\Migrate\Test\Helper;

use HHPack\Migrate\{ Connection, DatabaseClient };

function connect(): Connection {
    $host = getenv('DB_HOSTNAME');
    $name = getenv('DB_DATABASE');
    $port = getenv('DB_PORT');
    $user = getenv('DB_USER');
    $password = getenv('DB_PASSWORD');

    return \HH\Asio\join(DatabaseClient::createConnection(
        'mysql:host=' . $host . ';port=' . $port . ';dbname=' . $name,
        $user,
        $password
    ));
}
