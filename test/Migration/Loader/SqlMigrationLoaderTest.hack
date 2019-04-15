namespace HHPack\Migrate\Test\Migration\Loader;

use HHPack\Migrate\{File};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};

use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;

final class SqlMigrationLoaderTest extends HackTest {
  public function testLoadUpgradeMigrations(): void {
    $directory = File\absolute_path(__DIR__.'/../../sql/migrations/');

    $loader = new SqlMigrationLoader($directory);
    $migrations = $loader->loadUpgradeMigrations();

    expect($migrations->count())->toBeSame(2);
  }
}
