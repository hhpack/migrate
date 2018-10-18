<?hh //strict

namespace HHPack\Migrate\Test\Database;

use HHPack\Migrate\Test\Helper\{Db};
use HHPack\Migrate\Database\{Connection, MySqlConnection};
use type Facebook\HackTest\{HackTest,DataProvider};
use function Facebook\FBExpect\expect;

final class MySqlConnectionTest extends HackTest {
  public function provideConnection(): vec<(Connection)> {
    return vec[tuple(Db\connect())];
  }

  <<DataProvider('Db')>>
  public function testQuery(Connection $conn): void {
    $result = \HH\Asio\join($conn->rawQuery('show tables'));
    expect($result->isEmpty())->toBeTrue();
  }
}
