<?hh //strict

namespace HHPack\Migrate\Test\Migration\Loader;

use HHPack\Migrate\{File};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HackPack\HackUnit\Contract\Assert;

final class SqlMigrationLoaderTest {
  <<Test>>
  public function loadUpgradeMigrations(Assert $assert): void {
    $directory = File\absolutePath(__DIR__.'/../../sql/migrations/');

    $loader = new SqlMigrationLoader($directory);
    $migrations = $loader->loadUpgradeMigrations();

    $assert->int($migrations->count())->eq(2);
  }
}
