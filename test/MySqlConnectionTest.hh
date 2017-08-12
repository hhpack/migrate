<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper;
use HHPack\Migrate\{ Connection, MySqlConnection };
use HackPack\HackUnit\Contract\Assert;


final class MySqlConnectionTest
{
    public function __construct(
        private Connection $conn
    )
    {
    }

    <<SuiteProvider('Db')>>
    public static function create() : this
    {
        $conn = Helper\connect();
        return new static($conn);
    }

    <<Test('Db')>>
    public function query(Assert $assert): void
    {
        $result = \HH\Asio\join($this->conn->query('show tables'));
        $assert->string($result->query())->is('show tables');
    }
}
