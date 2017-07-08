<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\SqlMigrationLoader;
use HackPack\HackUnit\Contract\Assert;

final class SqlMigrationLoaderTest
{
    <<Test>>
    public function loadUpgradeMigrations(Assert $assert): void
    {
        $directory = realpath(__DIR__ . '/sql/migrations/');

        $loader = new SqlMigrationLoader($directory);
        $migrations = $loader->loadUpgradeMigrations();

        $assert->int($migrations->count())->eq(2);
    }
}
