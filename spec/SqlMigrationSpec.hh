<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\SqlMigration;
use hhpack\migrate\EventPublisher;
use hhpack\migrate\MySqlConnection;
use hhpack\migrate\MigratorAgent;
use hhpack\migrate\spec\helper;
use AsyncMysqlClient;

describe(SqlMigration::class, function() {
  beforeEach(function() {
    $this->sql = SqlMigration::fromFile(realpath(__DIR__ . '/sql/20150824010439-show-tables.up.sql'));
    $this->connection = new MySqlConnection(helper\connect());
    $this->agent = new MigratorAgent($this->connection, new EventPublisher());
  });
  describe('#version', function() {
    it('return version', function () {
      expect($this->sql->version())->toBe('20150824010439');
    });
  });
  describe('#name', function() {
    it('return name', function () {
      expect($this->sql->name())->toBe('20150824010439-show-tables');
    });
  });
  describe('#change', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join( $this->sql->change($this->agent) );
    });
    it('return QueryResult', function () {
      expect($this->result->query())->toBe("show tables;\n");
    });
  });
});
