<?hh //partial

namespace HHPack\Migrate\spec;

use HHPack\Migrate\spec\helper;
use HHPack\Migrate\MigrationManager;

describe(MigrationManager::class, function() {
  beforeEach(function() {
    $this->conn = helper\connect();
    $this->manager = new MigrationManager($this->conn);

    \HH\Asio\join( $this->conn->query('DROP TABLE IF EXISTS scheme_migrations') );
  });
  describe('#setup', function() {
    beforeEach(function() {
      \HH\Asio\join( $this->manager->setup() );
      $this->result = \HH\Asio\join( $this->conn->query('SHOW CREATE TABLE scheme_migrations') );
    });
    it('create migration log table', function () {
      $rows = $this->result->rows();
      expect($rows->count())->toBe(1);
    });
  });
  describe('#loadMigrations', function() {
    beforeEach(function() {
      \HH\Asio\join( $this->manager->setup() );
      \HH\Asio\join( $this->conn->query("INSERT INTO scheme_migrations (name, run_at) VALUES ('20150824010439.up.sql', CURRENT_TIMESTAMP)") );
      \HH\Asio\join( $this->conn->query("INSERT INTO scheme_migrations (name, run_at) VALUES ('20150825102100.up.sql', CURRENT_TIMESTAMP)") );
      $this->result = \HH\Asio\join( $this->manager->loadMigrations() );
      \HH\Asio\join( $this->conn->query("DELETE FROM scheme_migrations") );
    });
    it('load migration logs', function () {
      expect($this->result->count())->toBe(2);
    });
  });

});
