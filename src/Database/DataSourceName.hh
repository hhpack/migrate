<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

final class DataSourceName
{

    public function __construct(
        private DataSourceType $type,
        private string $name,
        private DatabaseServer $server
    )
    {
    }

    public function type(): DataSourceType
    {
        return $this->type;
    }

    public function name(): string
    {
        return $this->name;
    }

    public function host(): string
    {
        return $this->server->host();
    }

    public function port(): int
    {
        return $this->server->port();
    }

    public function server(): DatabaseServer
    {
        return $this->server;
    }

    public static function fromString(DSNString $dsn): this
    {
        $parts = explode(':', $dsn);
        $parameters = explode(';', $parts[1]);

        $dbname = '';
        $dbhost = DatabaseServer::DEFAULT_HOST;
        $dbport = DatabaseServer::DEFAULT_PORT;

        foreach ($parameters as $parameter) {
            list($name, $value) = explode('=', $parameter);

            if ($name === 'dbname') {
                $dbname = $value;
            } else if ($name === 'host') {
                $dbhost = $value;
            } else if ($name === 'port') {
                $dbport = (int) $value;
            }
        }
        $dbtype = DataSourceType::assert($parts[0]);
        $dbserver = new DatabaseServer($dbhost, $dbport);

        return new DataSourceName($dbtype, $dbname, $dbserver);
    }

}
