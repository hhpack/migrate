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

use HHPack\Migrate\{File};
use HHPack\Migrate\Migration\{MigrationType};
use HHPack\Migrate\Application\Configuration\{
  Configuration,
  Migration,
  Server,
  Loadable
};
use RuntimeException;

final class ConfigurationLoader implements Loadable {

  public function __construct(private string $path) {}

  public function load(string $env = 'development'): Configuration {
    $setting = File\read_json_file($this->path);

    if (is_null($setting['enviroments'])) {
      throw new RuntimeException('key enviroments not found');
    }

    $enviroments = $setting['enviroments'];

    if (!is_array($enviroments)) {
      throw new RuntimeException('enviroments is not object');
    }

    return new Configuration(
      $this->loadMigration($setting),
      $this->loadDatabaseServer($env, $enviroments),
    );
  }

  private function loadMigration(array<string, mixed> $setting): Migration {
    $loader = shape(
      "type" => MigrationType::assert((string) $setting['type']),
      "path" => getcwd().'/'.(string) $setting['path'],
    );

    return Migration::fromSetting($loader);
  }

  private function loadDatabaseServer(
    string $env,
    array<string, mixed> $setting,
  ): Server {
    if (is_null($setting[$env])) {
      throw new RuntimeException("$env is not found");
    }

    $environment = $setting[$env];

    if (!is_array($environment)) {
      throw new RuntimeException("$env is not found");
    }

    $serverSetting = $this->replaceEnvironmentVars($environment);

    $server = shape(
      "host" => (string) $serverSetting['host'],
      "port" => (int) $serverSetting['port'],
      "name" => (string) $serverSetting['name'],
      "user" => (string) $serverSetting['user'],
      "password" => (string) $serverSetting['password'],
    );

    return Server::fromSetting($server);
  }

  private function replaceEnvironmentVars(
    array<string, mixed> $setting,
  ): array<string, mixed> {
    $result = [];

    foreach ($setting as $key => $value) {
      if (!is_array($value)) {
        $result[$key] = $value;
        continue;
      }

      if (!array_key_exists('ENV', $value)) {
        $result[$key] = $this->replaceEnvironmentVars($value);
        continue;
      }

      $variable = getenv($value['ENV']);

      if ($variable === false) {
        throw new RuntimeException(
          "Environment variable {$value['ENV']} is not set",
        );
      } else {
        $result[$key] = $variable;
      }
    }

    return $result;
  }

}
