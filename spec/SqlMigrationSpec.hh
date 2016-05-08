<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\SqlMigration;
use hhpack\migrate\MySqlConnection;
use hhpack\migrate\MigratorAgent;
use hhpack\migrate\spec\helper;
use AsyncMysqlClient;

describe(SqlMigration::class, function() {
  beforeEach(function() {
    $this->sql = SqlMigration::fromFile(realpath(__DIR__ . '/sql/show_tables.sql'));
    $this->connection = new MySqlConnection(helper\connect());
    $this->agent = new MigratorAgent($this->connection);
  });
  describe('#change', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join( $this->sql->change($this->agent) );
    });
    it('return QueryResult', function () {
      expect($this->result->sql())->toBe("show tables;\n");
    });
  });
});
