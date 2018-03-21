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
use Facebook\TypeSpec;

final class ConfigurationLoader implements Loadable {
  public function __construct(private string $path) {}

  public function load(string $env = 'development'): Configuration {
    $setting = File\read_json_file($this->path);

    if (!\HH\Lib\C\contains_key($setting, 'enviroments')) {
      throw new RuntimeException('key enviroments not found');
    }
    $enviroments = $setting['enviroments'];
    $expectedEnviroments = $this->expectDict($enviroments);

    return new Configuration(
      $this->loadMigration($setting),
      $this->loadDatabaseServer($env, $expectedEnviroments),
    );
  }

  private function loadMigration(dict<string, mixed> $setting): Migration {
    $loader = shape(
      "type" => MigrationType::assert((string) $setting['type']),
      "path" => getcwd().'/'.(string) $setting['path'],
    );

    return Migration::fromSetting($loader);
  }

  private function loadDatabaseServer(
    string $env,
    dict<string, mixed> $setting,
  ): Server {
    if (is_null($setting[$env])) {
      throw new RuntimeException("$env is not found");
    }

    $environment = $setting[$env];
    $expectedEnvironment = $this->expectDict($environment);
    $serverSetting = $this->replaceEnvironmentVars($expectedEnvironment);

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
    dict<string, mixed> $setting,
  ): dict<string, mixed> {
    $result = dict[];

    foreach ($setting as $key => $value) {
      if (!is_dict($value)) {
        $result[$key] = $value;
        continue;
      }

      $expectedEnvVars = $this->expectDict($value);

      if (!\HH\Lib\C\contains_key($expectedEnvVars, 'ENV')) {
        $result[$key] = $this->replaceEnvironmentVars($expectedEnvVars);
        continue;
      }

      $result[$key] = $this->envvarFromDict($expectedEnvVars);
    }

    return $result;
  }

  private function expectDict(mixed $value): dict<string, mixed> {
    return TypeSpec\dict(TypeSpec\string(), TypeSpec\mixed())->assertType($value);
  }

  private function envvarFromDict(dict<string, mixed> $value): mixed {
    $envvarName = (string) $value['ENV'];
    $variable = getenv($envvarName);

    if ($variable === false) {
      throw new RuntimeException(
        "Environment variable {$envvarName} is not set",
      );
    }

    return $variable;
  }
}
