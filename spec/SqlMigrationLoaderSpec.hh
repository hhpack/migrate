<?hh //partial

namespace HHPack\Migrate\spec;

use HHPack\Migrate\SqlMigrationLoader;

describe(SqlMigrationLoader::class, function() {
  beforeEach(function() {
    $directory = realpath(__DIR__ . '/sql/migrations/');
    $this->loader = new SqlMigrationLoader($directory);
  });
  describe('#loadUpMigration', function() {
    beforeEach(function() {
      $this->migrations = $this->loader->loadUpgradeMigrations();
    });
    it('return migrations', function () {
      expect($this->migrations->count())->toBe(2);
    });
  });
});
