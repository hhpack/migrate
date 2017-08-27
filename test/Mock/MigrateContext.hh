<?hh //strict

namespace HHPack\Migrate\Test\Mock;

use HHPack\Migrate\{ Logger };
use HHPack\Migrate\Application\Context;
use HHPack\Migrate\Migration\{ MigrationLoader, SqlMigrationLoader };
use HHPack\Migrate\Database\{ Connection };
use HHPack\Migrate\Logger\{ PlainLogger, ColoredLogger };
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

    public function isSqlType() : bool
    {
        return true;
    }

    public function migrationPath() : string
    {
        return $this->path;
    }

    public function logger() : Logger {
        return new ColoredLogger();
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
