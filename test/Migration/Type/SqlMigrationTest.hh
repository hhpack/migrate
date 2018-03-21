<?hh //strict

namespace HHPack\Migrate\Test\Migration;

use HHPack\Migrate\Test\Helper\{Db};
use HHPack\Migrate\{File, MigratorAgent, FileNotFoundException};
use HHPack\Migrate\Event\{EventPublisher};
use HHPack\Migrate\Migration\Type\{SqlMigration};
use HHPack\Migrate\Database\{MySqlConnection};
use HackPack\HackUnit\Contract\Assert;

final class SqlMigrationTest {

  public function __construct(private MigratorAgent $agent) {}

  <<SuiteProvider('Db')>>
  public static function create(): this {
    $conn = Db\connect();
    $agent = new MigratorAgent($conn, new EventPublisher());

    return new static($agent);
  }

  <<Test('Db')>>
  public function migrationFile(Assert $assert): void {
    $path = File\absolute_path(
      __DIR__.'/../../sql/20150824010439-show-tables.up.sql',
    );
    $sql = SqlMigration::fromFile($path);

    $assert->string($sql->version())->is('20150824010439');
    $assert->string($sql->name())->is('20150824010439-show-tables');

    $results = \HH\Asio\join($sql->change($this->agent));
    $assert->int($results->count())->eq(1);
  }

  <<Test('Db')>>
  public function fileNotFoundMigrationFile(Assert $assert): void {
    $assert->whenCalled(
      () ==> {
        $path = File\absolute_path(__DIR__.'/../../sql/not_found.up.sql');
        $sql = SqlMigration::fromFile($path);
      },
    )->willThrowClass(FileNotFoundException::class);
  }
}
