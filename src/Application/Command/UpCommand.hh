<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Command;

use HHPack\Getopt as cli;
use HHPack\Getopt\Parser\{OptionParser};
use HHPack\Migrate\{Migrator};
use HHPack\Migrate\Application\{Context, Command, MigratorBuilder};
use HHPack\Migrate\Database\{Connection};

final class UpCommand extends MigrateSchemaCommand implements Command {
  private ?string $schemaVersion;

  public function __construct() {
    $this->usage = "migrate up [OPTIONS]";
    $this->description = "Upgrade the schema of the database";
    $this->optionParser = cli\optparser(
      [
        cli\take_on(
          ['--to'],
          'VERSION',
          'Version of the schema to be upgraded',
          ($version) ==> {
            $this->schemaVersion = $version;
          },
        ),
        cli\on(
          ['--dry-run'],
          'Test the change to apply',
          () ==> {
            $this->dryRun = true;
          },
        ),
        cli\on(
          ['-h', '--help'],
          'Display help message',
          () ==> {
            $this->help = true;
          },
        ),
      ],
    );
  }

  public function run(Context $context): void {
    $this->optionParser->parse($context->cliArgs());

    if ($this->help) {
      $this->displayHelp();
    } else {
      \HH\Asio\join($this->upgradeSchema($context));
    }
  }

  private async function upgradeSchema(Context $context): Awaitable<void> {
    $builder = new MigratorBuilder($context);
    $migrator = $builder->dryRun($this->dryRun)->build();

    if (\is_null($this->schemaVersion)) {
      await $migrator->upgrade();
    } else {
      await $migrator->upgradeTo($this->schemaVersion);
    }
  }
}
