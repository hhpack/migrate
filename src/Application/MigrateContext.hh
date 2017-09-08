<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Migrate\{Logger};
use HHPack\Migrate\Migration\{MigrationLoader};
use HHPack\Migrate\Migration\Loader\{SqlMigrationLoader};
use HHPack\Migrate\Database\{Connection, DatabaseClient, DatabaseServer};
use HHPack\Migrate\Application\Configuration\{
  Configuration,
  Migration,
  Server
};

final class MigrateContext implements Context {

  public function __construct(
    private Traversable<string> $args,
    private Configuration $config,
    private Logger $logger,
  ) {}

  public function cliArgs(): Traversable<string> {
    return $this->args;
  }

  public function migration(): Migration {
    return $this->config->migration();
  }

  public function logger(): Logger {
    return $this->logger;
  }

  public function databaseServer(): Server {
    return $this->config->databaseServer();
  }

}
