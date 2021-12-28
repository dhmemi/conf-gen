import * as React from "react";
import TreeView from "@mui/lab/TreeView";
import { styled } from "@mui/material/styles";
import ExpandMoreIcon from "@mui/icons-material/ExpandMore";
import ChevronRightIcon from "@mui/icons-material/ChevronRight";
import TreeItem, { treeItemClasses } from "@mui/lab/TreeItem";

import ItemCheck from "./ItemCheck"
import ItemInput from "./ItemInput";

type StyledTreeItemProps = {
  rootNode?: boolean;
};

const StyledTreeItem = styled(TreeItem)<StyledTreeItemProps>(({ rootNode }) => {
  const borderColor = "gray";

  return {
    position: "relative",
    "&:before": {
      pointerEvents: "none",
      content: '""',
      position: "absolute",
      width: 32,
      left: -16,
      top: 12,
      borderBottom:
        // only display if the TreeItem is not root node
        !rootNode ? `1px dashed ${borderColor}` : "none"
    },
    [`& .${treeItemClasses.group}`]: {
      marginLeft: 16,
      paddingLeft: 18,
      borderLeft: `1px dashed ${borderColor}`
    }
  };
});

interface Param {
  data: any,
}

export default class ParamTree extends React.Component<Param> {
  constructor(props: Param) {
    super(props)
  }

  changeValue(value: object) {
  }

  render() {
    return (
      <TreeView
        defaultCollapseIcon={<ExpandMoreIcon />}
        defaultExpandIcon={<ChevronRightIcon />}
        sx={{ height: 240, flexGrow: 1, m: 3 }}
      >
        <StyledTreeItem rootNode nodeId="0" label="Applications">
          {<StyledTreeItem nodeId="2" label={<ItemCheck data={this.props.data.enable} onClick={() => { }} />} />}
          <StyledTreeItem nodeId="3" label={<ItemInput data={this.props.data.height} onClick={() => { }} />} />
          <StyledTreeItem nodeId="4" label="Inbox">
            <StyledTreeItem nodeId="5" label="One" />
            <StyledTreeItem nodeId="6" label="Two" />
            <StyledTreeItem nodeId="7" label="Three" />
          </StyledTreeItem>
        </StyledTreeItem>
      </TreeView>
    )
  }
}