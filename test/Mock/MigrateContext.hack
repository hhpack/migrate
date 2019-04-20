namespace HHPack\Migrate\Test\Mock;

use HHPack\Migrate\{Logger};
use HHPack\Migrate\Application\Context;
use HHPack\Migrate\Application\Configuration\{Migration, Server};
use HHPack\Migrate\Migration\{MigrationType, MigrationLoader};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HHPack\Migrate\Database\{Connection};
use HHPack\Migrate\Logger\{PlainLogger, ColoredLogger};
use HHPack\Migrate\Test\Helper\{Db};

final class MigrateContext implements Context {

  public function __construct(
    private string $path,
    private Traversable<string> $args,
    private ?string $dbName = null,
  ) {}

  public function cliArgs(): Traversable<string> {
    return $this->args;
  }

  public function migration(): Migration {
    return new Migration(MigrationType::Sql, $this->path);
  }

  public function logger(): Logger {
    return new ColoredLogger();
  }

  public function databaseServer(): Server {
    return Db\connectServerSetting($this->dbName);
  }

}
