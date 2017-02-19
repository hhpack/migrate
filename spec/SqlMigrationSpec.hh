<?hh //partial

namespace HHPack\Migrate\spec;

use HHPack\Migrate\SqlMigration;
use HHPack\Migrate\EventPublisher;
use HHPack\Migrate\MySqlConnection;
use HHPack\Migrate\MigratorAgent;
use HHPack\Migrate\FileNotFoundException;
use HHPack\Migrate\spec\helper;

describe(SqlMigration::class, function() {
  beforeEach(function() {
    $this->sql = SqlMigration::fromFile(realpath(__DIR__ . '/sql/20150824010439-show-tables.up.sql'));
    $this->connection = helper\connect();
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
      $this->results = \HH\Asio\join( $this->sql->change($this->agent) );
    });
    it('return results', function () {
      expect($this->results)->toHaveLength(1);
    });
  });
  describe('#fromFile', function() {
    it('throw FileNotFoundException', function () {
      expect(function() {
        $sql = SqlMigration::fromFile(__DIR__ . '/sql/not_found.up.sql');
      })->toThrow(FileNotFoundException::class);
    });
  });
});
