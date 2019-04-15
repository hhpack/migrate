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

final class Server {

  public function __construct(
    private string $host,
    private int $port,
    private string $name,
    private string $user,
    private string $password,
  ) {}

  public function host(): string {
    return $this->host;
  }

  public function port(): int {
    return $this->port;
  }

  public function name(): string {
    return $this->name;
  }

  public function dns(): DSNString {
    return \sprintf(
      "mysql:dbname=%s;host=%s;port=%d",
      $this->name(),
      $this->host(),
      $this->port(),
    );
  }

  public function user(): string {
    return $this->user;
  }

  public function password(): string {
    return $this->password;
  }

  public static function fromSetting(DatabaseServer $server): this {
    return new self(
      $server['host'],
      $server['port'],
      $server['name'],
      $server['user'],
      $server['password'],
    );
  }
}
