/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

final class DatabaseServer {
  const DEFAULT_HOST = '127.0.0.1';
  const DEFAULT_PORT = 3306;

  public function __construct(private string $host, private int $port) {}

  public function host(): string {
    return $this->host;
  }

  public function port(): int {
    return $this->port;
  }

  public function __toString(): string {
    return \sprintf("%s:%d", $this->host, $this->port);
  }

}
