namespace HHPack\Migrate\Test\Database;

use HHPack\Migrate\Database\{DataSourceName};
use type Facebook\HackTest\HackTest;
use function Facebook\FBExpect\expect;

final class DataSourceNameTest extends HackTest {
  public function testCreate(): void {
    $dns = DataSourceName::fromString('mysql:dbname=test;port=10000');

    expect($dns->port())->toBeSame(10000);
    expect((string)$dns->type())->toBeSame('mysql');
    expect($dns->name())->toBeSame('test');
  }
}
