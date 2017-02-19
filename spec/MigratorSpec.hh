<?hh //partial

namespace HHPack\Migrate\spec;

use HHPack\Migrate\Migrator;
use HHPack\Migrate\SqlMigrationLoader;
use HHPack\Migrate\spec\helper;

describe(Migrator::class, function() {
  beforeEach(function() {
    $conn = helper\connect();
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

      $results = $this->result->at('20150825102100-create-posts');
      expect($results->at(0)->query())->toBe("DROP TABLE IF EXISTS `posts`");
    });
  });
});
