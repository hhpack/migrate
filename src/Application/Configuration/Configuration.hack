/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Configuration;

use HHPack\Migrate\Database\{DSNString};
use HHPack\Migrate\Migration\{MigrationType};

final class Configuration {

  public function __construct(
    private Migration $migration,
    private Server $server,
  ) {}

  public function isSqlType(): bool {
    return $this->migration->type() == MigrationType::Sql;
  }

  public function migrationType(): MigrationType {
    return $this->migration->type();
  }

  public function migrationPath(): string {
    return $this->migration->path();
  }

  public function migration(): Migration {
    return $this->migration;
  }

  public function databaseServer(): Server {
    return $this->server;
  }

  public function databaseDSN(): DSNString {
    return $this->server->dns();
  }

  public function databaseUser(): string {
    return $this->server->user();
  }

  public function databasePassword(): string {
    return $this->server->password();
  }

}
