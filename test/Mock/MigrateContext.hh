<?hh //strict

namespace HHPack\Migrate\Test\Mock;

use HHPack\Migrate\{ Connection, MigrationLoader, SqlMigrationLoader };
use HHPack\Migrate\Application\Context;
use HHPack\Migrate\Test\Helper;

final class MigrateContext implements Context
{

    public function __construct(
        private string $path,
        private Traversable<string> $args,
        private Connection $connection
    )
    {
    }

    public function cliArgs() : Traversable<string>
    {
        return $this->args;
    }

    public async function databaseClient() : Awaitable<Connection>
    {
        return await async {
            return $this->connection;
        };
    }

    public function migrationLoader() : MigrationLoader
    {
        return new SqlMigrationLoader($this->path);
    }
}
