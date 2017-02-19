<?hh //partial

namespace HHPack\Migrate\spec;

use HHPack\Migrate\MySqlConnection;
use HHPack\Migrate\spec\helper;

describe(MySqlConnection::class, function() {
  beforeEach(function() {
    $this->connection = helper\connect();
  });
  describe('#query', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join($this->connection->query('show tables'));
    });
    it('return QueryResult', function () {
      expect($this->result->query())->toBe('show tables');
    });
  });
});
