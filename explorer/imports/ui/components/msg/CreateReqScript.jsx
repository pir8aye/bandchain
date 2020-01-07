import React, { useState } from 'react'
import { Flex, Text } from 'rebass'
import styled from 'styled-components'
import SeperatorLine from '/imports/ui/components/SeperatorLine'

const Table = styled(Flex)`
  border-radius: 3px;
  border: solid 1px #dadada;
  background-color: #ffffff;
  flex-direction: column;
  width: 100%;
`

const TBody = styled(Flex)`
  padding: 0 18px;
  flex-direction: column;
`

const Title = styled(Flex)`
  font-size: 0.8em;
  color: black;
  width: 200px;
`

const Tab = styled(Flex)`
  justify-content: center;
  align-items: center;
  padding: 10px 15px;
  cursor: pointer;

  ${props =>
    props.active &&
    `
    border-bottom: 1px solid black;
  `}
`

export default ({ msg }) => {
  const { scriptHash, creator, name, sources, tag, code } = msg
  const [tabId, setTabId] = useState(0)
  return (
    <Table mb="20px">
      <Flex
        width="100%"
        flexDirection="row"
        style={{ borderBottom: '1px solid #ececec' }}
      >
        <Title style={{ marginRight: '18px', padding: '5px' }}>
          <Flex
            bg="#ececec"
            justifyContent="flex-start"
            pl="21px"
            alignItems="center"
            width="100%"
          >
            Create Data Request Script
          </Flex>
        </Title>
        <Tab active={tabId == 0} onClick={() => setTabId(0)}>
          Overview
        </Tab>
        <Tab active={tabId == 1} onClick={() => setTabId(1)}>
          Code
        </Tab>
      </Flex>
      <TBody>
        {tabId == 0 ? (
          <>
            <Flex flexDirection="row" mx="10px" my="13px">
              <Title>Script Hash:</Title>
              <Text fontSize="0.8em">{scriptHash}</Text>
            </Flex>
            <SeperatorLine color="#dadada" height="1px" my="0px" />
            <Flex flexDirection="row" mx="10px" my="13px">
              <Title>Creator:</Title>
              <Text fontSize="0.8em">{creator}</Text>
            </Flex>
            <SeperatorLine color="#dadada" height="1px" my="0px" />
            <Flex flexDirection="row" mx="10px" my="13px">
              <Title>Name:</Title>
              <Text fontSize="0.8em">{name}</Text>
            </Flex>
            <SeperatorLine color="#dadada" height="1px" my="0px" />
            <Flex flexDirection="row" mx="10px" my="13px">
              <Title>Data Sources:</Title>
              <Text fontSize="0.8em">{sources}</Text>
            </Flex>
            <SeperatorLine color="#dadada" height="1px" my="0px" />
            <Flex flexDirection="row" mx="10px" my="13px">
              <Title>Tags:</Title>
              <Text fontSize="0.8em">{tag}</Text>
            </Flex>
          </>
        ) : (
          <Flex
            justifyContent="center"
            alignItems="center"
            style={{ height: '220px' }}
          >
            {code}
          </Flex>
        )}
      </TBody>
    </Table>
  )
}