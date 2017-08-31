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
    /**
     * Default host name of database
     */
    const DEFAULT_HOST = '127.0.0.1';
    const DEFAULT_PORT = 3306;

    public function __construct(
        private ImmMap<string, mixed> $options
    )
    {
    }

    public function type(): DataSourceType
    {
        $type = $this->options->at('type');
        $dataSourceType = DataSourceType::assert($type);

        return $dataSourceType;
    }

    public function name(): string
    {
        return (string) $this->options->at('dbname');
    }

    public function host(): string
    {
        $host = $this->options->get('host');

        if ($host === null) {
            return static::DEFAULT_HOST;
        }

        return (string) $host;
    }

    public function port(): int
    {
        $port = $this->options->get('port');
        return ($port !== null) ? (int) $port : static::DEFAULT_PORT;
    }

    public static function fromString(DSNString $dsn): this
    {
        $options = Map {};
        $parts = explode(':', $dsn);
        $parameters = explode(';', $parts[1]);

        foreach ($parameters as $parameter) {
            $values = explode('=', $parameter);
            $options->set($values[0], $values[1]);
        }
        $options->set('type', $parts[0]);

        return new DataSourceName( $options->toImmMap() );
    }

}
