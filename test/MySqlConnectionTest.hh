<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\test\helper;
use HHPack\Migrate\Connection;
use HHPack\Migrate\MySqlConnection;
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
        $conn = helper\connect();
        return new static($conn);
    }

    <<Test('Db')>>
    public function query(Assert $assert): void
    {
        $result = \HH\Asio\join($this->conn->query('show tables'));
        $assert->string($result->query())->is('show tables');
    }
}
