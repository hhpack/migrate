<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\spec\helper;
use hhpack\migrate\Migrator;
use hhpack\migrate\SqlMigrationLoader;
use hhpack\migrate\MySqlConnection;
use AsyncMysqlClient;

describe(Migrator::class, function() {
  beforeEach(function() {
    $conn = new MySqlConnection(helper\connect());
    $loader = new SqlMigrationLoader(__DIR__ . '/sql/migrations');

    $this->migrator = new Migrator($loader, $conn);

    \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS scheme_migrations") );
    \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS users") );
    \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS posts") );
  });
  describe('#upgrade', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join( $this->migrator->upgrade() );
    });
    it('return MigrationResult', function () {
      expect($this->result->results())->toHaveLength(2);
    });
  });
  describe('#downgrade', function() {
    beforeEach(function() {
      \HH\Asio\join( $this->migrator->upgrade() );
      $this->result = \HH\Asio\join( $this->migrator->downgrade('20150824010439-create-users') );
    });
    it('downgrade to version', function () {
      expect($this->result->resultCount())->toBe(1);

      $result = $this->result->at(0);
      expect($result->query())->toBe("DROP TABLE IF EXISTS `posts`;\n");
    });
  });
});
