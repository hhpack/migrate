/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Configuration;

use HHPack\Migrate\Migration\{MigrationType};

final class Migration {

  public function __construct(
    private MigrationType $type,
    private string $path,
  ) {}

  public function type(): MigrationType {
    return $this->type;
  }

  public function path(): string {
    return $this->path;
  }

  public static function fromSetting(MigrationLoader $migration): this {
    return new self($migration['type'], $migration['path']);
  }

}
