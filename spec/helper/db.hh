<?hh //strict

namespace HHPack\Migrate\spec\helper;

use HHPack\Migrate\Connection;
use HHPack\Migrate\DatabaseClient;

function connect(): Connection {
  return \HH\Asio\join(DatabaseClient::createConnection(
    'mysql:host=127.0.0.1;port=3306;dbname=migrate',
    'migrate',
    'migrate'
  ));
}
