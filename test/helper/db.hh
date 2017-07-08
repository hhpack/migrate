<?hh //strict

namespace HHPack\Migrate\test\helper;

use HHPack\Migrate\Connection;
use HHPack\Migrate\DatabaseClient;

function connect(): Connection {
    $host = getenv('DB_HOSTNAME');
    $name = getenv('DB_DATABASE');
    $user = getenv('DB_USER');
    $password = getenv('DB_PASSWORD');

    return \HH\Asio\join(DatabaseClient::createConnection(
        'mysql:host=' . $host . ';port=3306;dbname=' . $name,
        $user,
        $password
    ));
}
