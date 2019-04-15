/**
 * This file is part of hhpack/migrate.
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Migrate\{Migrator, DryRunMigrator, Migratable};
use HHPack\Migrate\Migration\{MigrationLoader};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HHPack\Migrate\Database\{Connection, DatabaseClient};
use HHPack\Migrate\Application\{Context};
use HHPack\Migrate\Application\Configuration\{Migration, Server};
use RuntimeException;

final class MigratorBuilder {
  private bool $druRun = false;

  public function __construct(private Context $context) {}

  public function dryRun(bool $dryRun): this {
    $this->druRun = $dryRun;
    return $this;
  }

  public function build(): Migratable {
    $server = $this->context->databaseServer();
    $connection = $this->connectToServer($server);

    $loader = $this->loaderFrom($this->context->migration());

    if ($this->druRun) {
      return new DryRunMigrator($loader, $connection, $this->context->logger());
    } else {
      return new Migrator($loader, $connection, $this->context->logger());
    }
  }

  private function loaderFrom(Migration $setting): MigrationLoader {
    return new SqlMigrationLoader($setting->path());
  }

  private function connectToServer(Server $server): Connection {
    try {
      $connectionHandle = DatabaseClient::createConnection(
        $server->dns(),
        $server->user(),
        $server->password(),
      );
      return \HH\Asio\join($connectionHandle);
    } catch (\AsyncMysqlConnectException $e) {
      throw new \RuntimeException($e->getMessage(), $e->getCode());
    }
  }
}
