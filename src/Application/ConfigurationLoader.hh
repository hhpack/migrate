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

use HHPack\Migrate\{ MigrationType };
use HHPack\Migrate\Application\Configuration\{ Configuration, Migration, Server, Loadable };
use RuntimeException;

final class ConfigurationLoader implements Loadable
{

    public function __construct(
        private string $path
    )
    {
    }

    public function load(string $env = 'development') : Configuration
    {
        $json = file_get_contents($this->path);
        $setting = json_decode($json, true);

        return new Configuration(
            $this->loadMigration($setting),
            $this->loadDatabaseServer($env, $setting['enviroments'])
        );
    }

    private function loadMigration(array<string, mixed> $setting) : Migration
    {
        $loader = shape(
            "type" => MigrationType::assert((string) $setting['type']),
            "path" => (string) $setting['path']
        );

        return Migration::fromSetting($loader);
    }

    private function loadDatabaseServer(string $env, array<string, mixed> $setting) : Server
    {
        if (is_null($setting[$env])) {
            throw new RuntimeException("$env is not found");
        }

        $environment = $setting[$env];

        if (!is_array($environment)) {
            throw new RuntimeException("$env is not found");
        }

        $server = shape(
            "host" => (string) $environment['host'],
            "port" => (int) $environment['port'],
            "name" => (string) $environment['name'],
            "user" => (string) $environment['user'],
            "password" => (string) $environment['password']
        );

        return Server::fromSetting($server);
    }

}
