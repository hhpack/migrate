namespace HHPack\Migrate\Test\Migration;

use HHPack\Migrate\Test\Helper\{Db};
use HHPack\Migrate\{File, MigratorAgent, FileNotFoundException};
use HHPack\Migrate\Event\{EventPublisher};
use HHPack\Migrate\Migration\Type\{SqlMigration};
use HHPack\Migrate\Database\{MySqlConnection};

use type Facebook\HackTest\{HackTest, DataProvider};
use function Facebook\FBExpect\expect;

final class SqlMigrationTest extends HackTest {

  public function provideAgent(): vec<(MigratorAgent)> {
    $conn = Db\connect();
    $agent = new MigratorAgent($conn, new EventPublisher());

    return vec[tuple($agent)];
  }

  <<DataProvider('provideAgent')>>
  public function testMigrationFile(MigratorAgent $agent): void {
    $path = File\absolute_path(
      __DIR__.'/../../sql/20150824010439-show-tables.up.sql',
    );
    $sql = SqlMigration::fromFile($path);

    expect($sql->version())->toBeSame('20150824010439');
    expect($sql->name())->toBeSame('20150824010439-show-tables');

    $results = \HH\Asio\join($sql->change($agent));
    expect($results->count())->toBeSame(1);
  }

  public function testFileNotFoundMigrationFile(): void {
    expect(() ==> {
      $path = File\absolute_path(__DIR__.'/../../sql/not_found.up.sql');
      $sql = SqlMigration::fromFile($path);
    })->toThrow(FileNotFoundException::class);
  }
}
