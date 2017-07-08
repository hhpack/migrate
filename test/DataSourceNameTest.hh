<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\DataSourceName;
use HackPack\HackUnit\Contract\Assert;

final class DataSourceNameTest
{
    <<Test>>
    public function create(Assert $assert): void
    {
        $dns = DataSourceName::fromString('mysql:dbname=test;port=10000');

        $assert->int($dns->port())->eq(10000);
        $assert->string((string) $dns->type())->is('mysql');
        $assert->string($dns->name())->is('test');
    }
}
