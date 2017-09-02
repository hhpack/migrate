<?hh //strict

namespace HHPack\Migrate\Test\Mock;

use HHPack\Migrate\{ Logger };
use HHPack\Migrate\Application\Context;
use HHPack\Migrate\Application\Configuration\{ Server };
use HHPack\Migrate\Migration\{ MigrationLoader };
use HHPack\Migrate\Migration\Loader\{ SqlMigrationLoader };
use HHPack\Migrate\Database\{ Connection };
use HHPack\Migrate\Logger\{ PlainLogger, ColoredLogger };
use HHPack\Migrate\Test\Helper\{ Db };

final class MigrateContext implements Context
{

    public function __construct(
        private string $path,
        private Traversable<string> $args,
        private Connection $connection,
        private ?string $dbName = null
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

    public function databaseServer(): Server
    {
        return Db\connectServerSetting($this->dbName);
    }

    public function connectDatabase() : Connection
    {
        return $this->connection;
    }

    public function migrationLoader() : MigrationLoader
    {
        return new SqlMigrationLoader($this->path);
    }
}
