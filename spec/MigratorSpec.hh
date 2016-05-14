<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\spec\helper;
use hhpack\migrate\Migrator;
use hhpack\migrate\MigrationLoader;
use hhpack\migrate\SqlMigrationLoader;
use hhpack\migrate\MySqlConnection;
use AsyncMysqlClient;

describe(Migrator::class, function() {
  beforeEach(function() {
    $conn = new MySqlConnection(helper\connect());
    $loader = new MigrationLoader(new SqlMigrationLoader(__DIR__ . '/sql/migrations'));

    $this->migrator = new Migrator($loader, $conn);

    \HH\Asio\join( $this->migrator->downgrade() );
  });
  describe('#upgrade', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join( $this->migrator->upgrade() );
    });
    it('return MigrationResult', function () {
      expect($this->result->results())->toHaveLength(2);
    });
  });
});
