<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\MySqlConnection;
use hhpack\migrate\spec\helper;
use AsyncMysqlClient;

describe(MySqlConnection::class, function() {
  beforeEach(function() {
    $this->connection = new MySqlConnection(helper\connect());
  });
  describe('#query', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join($this->connection->query('show tables'));
    });
    it('return QueryResult', function () {
      expect($this->result->sql())->toBe('show tables');
    });
  });
});
