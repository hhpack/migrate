<?hh //strict

namespace hhpack\migrate\spec\helper;

use AsyncMysqlClient;
use AsyncMysqlConnection;

function connect(): AsyncMysqlConnection {
  return \HH\Asio\join(AsyncMysqlClient::connect('127.0.0.1', 3306, 'migrate', 'migrate', 'migrate'));
}
