<?hh //strict

namespace hhpack\migrate\spec\helper;

use hhpack\migrate\Connection;
use hhpack\migrate\DatabaseClient;

function connect(): Connection {
  return \HH\Asio\join(DatabaseClient::createConnection(
    'mysql:host=127.0.0.1;port=3306;dbname=migrate',
    'migrate',
    'migrate'
  ));
}
