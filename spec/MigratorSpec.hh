<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\spec\helper;
use hhpack\migrate\Migrator;
use hhpack\migrate\MigratorAgent;
use hhpack\migrate\MigrationLoader;
use hhpack\migrate\SqlMigrationLoader;
use hhpack\migrate\MySqlConnection;
use AsyncMysqlClient;

describe(Migrator::class, function() {
  beforeEach(function() {
    $connection = new MySqlConnection(helper\connect());
    $agent = new MigratorAgent($connection);

    $loader = new MigrationLoader(new SqlMigrationLoader(__DIR__ . '/sql/migrations'));

    $this->migrator = new Migrator($loader, $agent);

    \HH\Asio\join( $agent->query('DROP TABLE IF EXISTS users') );
  });
  describe('#upgrade', function() {
    beforeEach(function() {
      $this->result = \HH\Asio\join( $this->migrator->upgrade() );
    });
    it('return MigrationResult', function () {
      expect($this->result->results())->toHaveLength(1);
    });
  });
});
