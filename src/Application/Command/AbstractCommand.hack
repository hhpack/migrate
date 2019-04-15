/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Command;

use HHPack\Migrate\Application\{Command};
use HHPack\Getopt\Parser\{OptionParser};

abstract class AbstractCommand implements Command {
  protected string $usage;
  protected string $description;

  protected bool $help = false;
  protected OptionParser $optionParser;

  public function usage(): string {
    return $this->usage;
  }

  public function description(): string {
    return $this->description;
  }

  protected function displayHelp(): void {
    \fwrite(\STDOUT, $this->description);
    \fwrite(\STDOUT, \sprintf("\n\n  %s\n\n", $this->usage));
    $this->optionParser->displayHelp();
  }

}
